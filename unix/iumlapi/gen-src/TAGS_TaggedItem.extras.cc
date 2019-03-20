//
// UK Crown Copyright (c) 2005. All Rights Reserved
//
iUMLAPI::TAGS::TagAttachment iUMLAPI::TAGS::TaggedItem::attachTag ( Tag& tag, const std::string& tagValue )
{  
  reset();
  tag.reset();
  API_ENTITY newAttachmentEntity;
  iUMLAPI::attach_tag(entity.getEntity(),tag.getEntity().getEntity(),newAttachmentEntity);
  iUMLAPI::define_tag_value(newAttachmentEntity,tagValue);
  return TagAttachment(newAttachmentEntity);
}

void iUMLAPI::TAGS::TaggedItem::detachTag ( Tag& tag )
{  
  iUMLAPI::detach_tag(entity.getEntity(),tag.getEntity().getEntity());
  reset();
  tag.reset();
}
