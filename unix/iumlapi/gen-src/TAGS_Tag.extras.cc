//
// UK Crown Copyright (c) 2016. All Rights Reserved.
//
iUMLAPI::TAGS::TagAttachment iUMLAPI::TAGS::Tag::attachToItem ( TaggedItem& item, const std::string& tagValue )
{  
  reset();
  item.reset();
  API_ENTITY newAttachmentEntity;
  iUMLAPI::attach_tag(item.getEntity().getEntity(),entity.getEntity(),newAttachmentEntity);
  iUMLAPI::define_tag_value(newAttachmentEntity,tagValue);
  return TagAttachment(newAttachmentEntity);
}

void iUMLAPI::TAGS::Tag::detachFromItem ( TaggedItem& item )
{  
  iUMLAPI::detach_tag(item.getEntity().getEntity(),entity.getEntity());
  reset();
  item.reset();
}

